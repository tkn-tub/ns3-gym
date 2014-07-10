/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: George F. Riley<riley@ece.gatech.edu>
 * Author: John Abraham <john.abraham@gatech.edu>
 * Contributions: Eugene Kalishenko <ydginster@gmail.com> (Open Source and Linux Laboratory http://dev.osll.ru/)
 */

// Interface between ns3 and the network animator

#ifndef ANIMATION_INTERFACE__H
#define ANIMATION_INTERFACE__H

#include <string>
#include <cstdio>
#include <map>

#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/node-container.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/node-list.h"
#include "ns3/random-variable-stream.h"
#include "ns3/simulator.h"
#include "ns3/config.h"
#include "ns3/mac48-address.h"
#include "ns3/lte-ue-net-device.h"
#include "ns3/lte-enb-net-device.h"
#include "ns3/uan-phy-gen.h"
#include "ns3/rectangle.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-l3-protocol.h"

namespace ns3 {

#define MAX_PKTS_PER_TRACE_FILE 100000
#define PURGE_INTERVAL 5
#define NETANIM_VERSION "netanim-3.105"


struct NodeSize;

/**
 * \defgroup netanim Network Animation
 *
 * This section documents the API of the ns-3 netanim module. For a generic functional description, please refer to the ns-3 manual.
 */

/**
 * \ingroup netanim
 *
 * \brief Interface to network animator
 *
 * Provides functions that facilitate communications with an
 * external or internal network animator.
 */
class AnimationInterface
{
public:

  /**
   * \brief Constructor
   * \param filename The Filename for the trace file used by the Animator
   *
   */
  AnimationInterface (const std::string filename);

  /**
   * Counter Types 
   */
  typedef enum
    {
      UINT32_COUNTER,
      DOUBLE_COUNTER
    } CounterType;

  /**
   * \brief typedef for WriteCallBack used for listening to AnimationInterface
   * write messages
   * 
   */
  typedef void (*AnimWriteCallback) (const char * str);

  /**
   * \brief Destructor for the animator interface.
   *
   */
  ~AnimationInterface ();

  /**
   * \brief Enable tracking of Ipv4 L3 Protocol Counters such as Tx, Rx, Drop
   *
   * \param startTime Start Time for capturing values
   * \param stopTime Stop Time for capturing values
   * \param pollInterval The periodic interval at which the counters are written to the trace file
   *        Default: 1s
   * \returns none
   */
  void EnableIpv4L3ProtocolCounters (Time startTime, Time stopTime, Time pollInterval = Seconds (1));

  /**
   * \brief Enable tracking of Queue Counters such as Enqueue, Dequeue, Queue Drops
   *
   * \param startTime Start Time for capturing values
   * \param stopTime Stop Time for capturing values
   * \param pollInterval The periodic interval at which the counters are written to the trace file
   *        Default: 1s
   * \returns none
   */
  void EnableQueueCounters (Time startTime, Time stopTime, Time pollInterval = Seconds (1));

  /**
   * \brief Enable tracking of Wifi Mac Counters such as Tx, TxDrop, Rx, RxDrop
   *
   * \param startTime Start Time for capturing values
   * \param stopTime Stop Time for capturing values
   * \param pollInterval The periodic interval at which the counters are written to the trace file
   *        Default: 1s
   * \returns none
   */
  void EnableWifiMacCounters (Time startTime, Time stopTime, Time pollInterval = Seconds (1));

  /**
   * \brief Enable tracking of Wifi Phy Counters such as TxDrop, RxDrop
   *
   * \param startTime Start Time for capturing values
   * \param stopTime Stop Time for capturing values
   * \param pollInterval The periodic interval at which the counters are written to the trace file
   *        Default: 1s
   * \returns none
   */
  void EnableWifiPhyCounters (Time startTime, Time stopTime, Time pollInterval = Seconds (1));

  /**
   * \brief Enable tracking of the Ipv4 routing table for all Nodes
   *
   * \param fileName Trace file for storing routing table information 
   * \param startTime Start time for capture
   * \param stopTime  End time for capture
   * \param pollInterval The periodic interval at which routing table information is polled
   *        Default: 5s
   *
   * \returns reference to this AnimationInterface object
   */
  AnimationInterface & EnableIpv4RouteTracking (std::string fileName, Time startTime, Time stopTime, Time pollInterval = Seconds (5));

  /**
   * \brief Enable tracking of the Ipv4 routing table for a set of Nodes
   *
   * \param fileName Trace file for storing routing table information
   * \param startTime Start time for capture
   * \param stopTime  End time for capture
   * \param nc A NodeContainer containing nodes for which Routing table has to be tracked
   * \param pollInterval The periodic interval at which routing table information is polled
   *        Default: 5s
   *
   * \returns reference to this AnimationInterface object
   */
  AnimationInterface & EnableIpv4RouteTracking (std::string fileName, Time startTime, Time stopTime, NodeContainer nc, Time pollInterval = Seconds (5));

  /**
   * \brief Check if AnimationInterface is initialized
   *
   * \returns true if AnimationInterface was already initialized
   *
   */
  static bool IsInitialized (void);

  /**
   * \brief Specify the time at which capture should start
   * 
   * \param t The time at which AnimationInterface should begin capture of traffic info
   *
   * \returns none
   */
  void SetStartTime (Time t);

  /**
   * \brief Specify the time at which capture should stop
   * 
   * \param t The time at which AnimationInterface should stop capture of traffic info
   *
   * \returns none
   */
  void SetStopTime (Time t);

  /**
   * \brief Set Max packets per trace file
   * \param maxPktsPerFile The maximum number of packets per trace file.
	    AnimationInterface will create trace files with the following 
            filenames : filename, filename-1, filename-2..., filename-N
	    where each file contains packet info for 'maxPktsPerFile' number of packets
   *
   * \returns none
   */
  void SetMaxPktsPerTraceFile (uint64_t maxPktsPerFile);

  /**
   * \brief Set mobility poll interval:WARNING: setting a low interval can 
   * cause slowness
   *
   * \param t Time interval between fetching mobility/position information
   * Default: 0.25s
   *
   * \returns none
   */
  void SetMobilityPollInterval (Time t);

  /**
   * \brief Set a callback function to listen to AnimationInterface write events
   *
   * \param cb Address of callback function
   *
   * \returns none
   */
  void SetAnimWriteCallback (AnimWriteCallback cb);

  /**
   * \brief Reset the write callback function
   *
   * \returns none
   */
  void ResetAnimWriteCallback ();

  /**
   * \brief Helper function to set Constant Position for a given node
   * \param n Ptr to the node
   * \param x X co-ordinate of the node
   * \param y Y co-ordinate of the node
   * \param z Z co-ordinate of the node
   *
   * \returns none
   */
  static void SetConstantPosition (Ptr <Node> n, double x, double y, double z=0);

  /**
   * \brief Helper function to update the description for a given node
   * \param n Ptr to the node
   * \param descr A string to briefly describe the node
   *
   * \returns none
   */
  void UpdateNodeDescription (Ptr <Node> n, std::string descr);

  /**
   * \brief Helper function to update the description for a given node
   * \param nodeId Id of the node
   * \param descr A string to briefly describe the node
   *
   * \returns none
   */
  void UpdateNodeDescription (uint32_t nodeId, std::string descr);

  /**
   * \brief Helper function to update the image of a node
   * \param nodeId Id of the node
   * \param resourceId Id of the image resource that was previously added
   *
   * \returns none
   */
  void UpdateNodeImage (uint32_t nodeId, uint32_t resourceId);

  /**
   * \brief Helper function to update the size of a node
   * \param nodeId Id of the node
   * \param width Width of the node
   * \param height Height of the node
   *
   * \returns none
   */
  void UpdateNodeSize (uint32_t nodeId, double width, double height); 

  /**
   * \brief Helper function to update the node color
   * \param n Ptr to the node
   * \param r Red component value (0-255)
   * \param g Green component value (0-255)
   * \param b Blue component value (0-255)
   *
   * \returns none
   */
  void UpdateNodeColor (Ptr <Node> n, uint8_t r, uint8_t g, uint8_t b);

  /**
   * \brief Helper function to update the node color
   * \param nodeId Id of the node
   * \param r Red component value (0-255)
   * \param g Green component value (0-255)
   * \param b Blue component value (0-255)
   *
   * \returns none
   */
  void UpdateNodeColor (uint32_t nodeId, uint8_t r, uint8_t g, uint8_t b);

  /**
   * \brief Helper function to update a node's counter referenced by the nodeCounterId
   * \param nodeCounterId The counter Id obtained from AddNodeCounter
   * \param nodeId Node Id of the node
   * \param counter Current value of the counter
   *
   * \returns none
   */
  void UpdateNodeCounter (uint32_t nodeCounterId, uint32_t nodeId, double counter);

  /**
   * \brief Helper function to set the background image
   * \param fileName File name of the background image
   * \param x X co-ordinate of the image
   * \param y Y co-ordinate of the image
   * \param scaleX X scale of the image
   * \param scaleY Y scale of the image
   * \param opacity Opacity of the background: A value between 0.0 and 1.0. 0.0 is transparent,
   *        1.0 is opaque
   *
   * \returns none
   */
  void SetBackgroundImage (std::string fileName, double x, double y, double scaleX, double scaleY, double opacity); 

  /**
   * \brief Helper function to update the description for a link
   * \param fromNode Node Id of the "from Node" of the p2p link
   * \param toNode Node Id of the "to Node" of the p2p link
   * \param linkDescription Description of the link such as link bandwidth
   *
   * \returns none
   */
  void UpdateLinkDescription (uint32_t fromNode, uint32_t toNode,
                              std::string linkDescription);

  /**
   * \brief Helper function to update the description for a link
   * \param fromNode Ptr to the "from Node" of the p2p link
   * \param toNode Ptr to the "to Node" of the p2p link
   * \param linkDescription Description of the link such as link bandwidth
   *
   * \returns none
   */
  void UpdateLinkDescription (Ptr <Node> fromNode, Ptr <Node> toNode,
                              std::string linkDescription);

  /**
   * \brief Helper function to print the routing path from a source node to destination IP
   * \param fromNodeId The source node
   * \param destinationIpv4Address The destination Ipv4 Address 
   *
   * \returns reference to this AnimationInterface object
   */
  AnimationInterface & AddSourceDestination (uint32_t fromNodeId, std::string destinationIpv4Address);

  /**
   * \brief Is AnimationInterface started
   *
   * \returns true if AnimationInterface was started
   */
  bool IsStarted (void);

  /**
   * \brief Do not trace packets. This helps reduce the trace file size if AnimationInterface is solely
   *        used for tracking mobility, routing paths and counters
   * \returns none
   */
  void SkipPacketTracing ();

  /**
   *
   * \brief Enable Packet metadata
   * \param enable if true enables writing the packet metadata to the XML trace file
   *        if false disables writing the packet metadata
   *
   * \returns none
   */
  void EnablePacketMetadata (bool enable = true);

  /**
   *
   * \brief Get trace file packet count (This used only for testing)
   *
   * returns Number of packets recorded in the current trace file
   */
  uint64_t GetTracePktCount ();

  /**
   *
   * \brief Setup a node counter
   * \param counterName A string to identify the counter
   * \param counterType The type of the counter, such as uint32, double etc
   * 
   * returns The id of the counter to be used as a reference for future
   */
  uint32_t AddNodeCounter (std::string counterName, CounterType counterType); 

  /**
   *
   * \brief Add a resource such as the path to an image file
   * \param resourcePath Absolute Path to an image/resource
   * 
   * returns a number identifying the resource
   */
  uint32_t AddResource (std::string resourcePath);

  /**
   *
   * \brief Get node's energy fraction (This used only for testing)
   *
   * returns current node's remaining energy (between [0, 1])
   */
  double GetNodeEnergyFraction (Ptr <const Node> node) const;

private:


  // ##### typedef #####
  class AnimRxInfo
  {
  public:
    AnimRxInfo () {}
    AnimRxInfo (const Time& fbRx, Ptr <const NetDevice> nd ,double rxRange)
      : m_fbRx (fbRx.GetSeconds ()), m_lbRx (0), m_rxnd (nd), rxRange (rxRange), m_PhyRxComplete (false) {}
    double m_fbRx;            
    double m_lbRx;             
    Ptr <const NetDevice> m_rxnd;
    double rxRange;
    bool IsPhyRxComplete ();
    void SetPhyRxComplete ();
  private:
    bool m_PhyRxComplete;
  };

  class AnimPacketInfo
  
  {
  public:
    AnimPacketInfo ();
    AnimPacketInfo (const AnimPacketInfo & pInfo);
    AnimPacketInfo(Ptr <const NetDevice> tx_nd, const Time fbTx, const Time lbTx, Vector txLoc, uint32_t txNodeId = 0);
    Ptr <const NetDevice> m_txnd;
    uint32_t m_txNodeId;
    double m_fbTx;     
    double m_lbTx;     
    Vector m_txLoc;
    double m_firstLastBitDelta;
    std::map<uint32_t,AnimRxInfo> m_rx;
    void ProcessRxBegin (Ptr <const NetDevice> nd, const Time fbRx);
    bool ProcessRxEnd (Ptr <const NetDevice> nd, const Time fbRx, Vector rxLoc);
    void ProcessRxDrop (Ptr <const NetDevice> nd);
    AnimRxInfo GetRxInfo (Ptr <const NetDevice> nd);
    void RemoveRxInfo (Ptr <const NetDevice> nd);
  
  };

  typedef struct
    {
      uint8_t r;
      uint8_t g;
      uint8_t b;
    } Rgb;

  typedef struct
  {
    uint32_t fromNode;
    uint32_t toNode;
  } P2pLinkNodeIdPair;

  typedef struct
  {
    std::string fromNodeDescription;
    std::string toNodeDescription;
    std::string linkDescription;
  } LinkProperties;

  struct LinkPairCompare
  {
    bool operator () (P2pLinkNodeIdPair first, P2pLinkNodeIdPair second) const
      {
        //Check if they are the same node pairs but flipped
        if (  ((first.fromNode == second.fromNode) && (first.toNode == second.toNode)) ||
              ((first.fromNode == second.toNode) && (first.toNode == second.fromNode)) )
          {
            return false;
          }
        std::ostringstream oss1;
        oss1 << first.fromNode << first.toNode;
        std::ostringstream oss2;
        oss2 << second.fromNode << second.toNode;
        return oss1.str () < oss2.str ();
      }

  };

  typedef struct 
    {
      std::string destination;
      uint32_t fromNodeId;
    } Ipv4RouteTrackElement;

  typedef struct 
    {
      uint32_t nodeId;
      std::string nextHop;
    } Ipv4RoutePathElement;

  typedef enum
    {
      UAN,
      LTE,
      WIFI,
      WIMAX,
      CSMA
    } ProtocolType;

  typedef struct
    {
      double width;
      double height;
    } NodeSize;
  typedef std::map <P2pLinkNodeIdPair, LinkProperties, LinkPairCompare> LinkPropertiesMap;
  typedef std::map <uint32_t, std::string> NodeDescriptionsMap;
  typedef std::map <uint32_t, Rgb> NodeColorsMap;
  typedef std::map<uint64_t, AnimPacketInfo> AnimUidPacketInfoMap;
  typedef std::map <uint32_t, double> EnergyFractionMap;
  typedef std::vector <Ipv4RoutePathElement> Ipv4RoutePathElements;


  // Node Counters
  typedef std::map <uint32_t, uint64_t> NodeCounterMap64;


  class AnimXmlElement
  {
    public:
    AnimXmlElement (std::string tagName);
    template <typename T>
    void AddAttribute (std::string attribute, T value);
    void Close ();
    void CloseTag ();
    void AddLineBreak ();
    void Add (AnimXmlElement e);
    std::string GetElementString ();
  private:
    std::string m_tagName;
    std::string m_elementString;

  };


  // ##### State #####

  FILE * m_f; // File handle for output (0 if none)
  FILE * m_routingF; // File handle for routing table output (0 if None);
  Time m_mobilityPollInterval;
  std::string m_outputFileName;
  uint64_t gAnimUid ;    // Packet unique identifier used by AnimationInterface
  AnimWriteCallback m_writeCallback;
  bool m_started;
  bool m_enablePacketMetadata; 
  Time m_startTime;
  Time m_stopTime;
  uint64_t m_maxPktsPerFile;
  std::string m_originalFileName;
  Time m_routingStopTime;
  std::string m_routingFileName;
  Time m_routingPollInterval;
  NodeContainer m_routingNc;
  Time m_ipv4L3ProtocolCountersStopTime;
  Time m_ipv4L3ProtocolCountersPollInterval;
  Time m_queueCountersStopTime;
  Time m_queueCountersPollInterval;
  Time m_wifiMacCountersStopTime;
  Time m_wifiMacCountersPollInterval;
  Time m_wifiPhyCountersStopTime;
  Time m_wifiPhyCountersPollInterval;
  static Rectangle * userBoundary;
  bool m_trackPackets;

  // Counter ID
  uint32_t m_remainingEnergyCounterId;
  
  uint32_t m_ipv4L3ProtocolTxCounterId;
  uint32_t m_ipv4L3ProtocolRxCounterId;
  uint32_t m_ipv4L3ProtocolDropCounterId;

  uint32_t m_queueEnqueueCounterId;
  uint32_t m_queueDequeueCounterId;
  uint32_t m_queueDropCounterId;

  uint32_t m_wifiMacTxCounterId;
  uint32_t m_wifiMacTxDropCounterId;
  uint32_t m_wifiMacRxCounterId;
  uint32_t m_wifiMacRxDropCounterId;
 
  uint32_t m_wifiPhyTxDropCounterId;
  uint32_t m_wifiPhyRxDropCounterId;
  
  AnimUidPacketInfoMap m_pendingWifiPackets;
  AnimUidPacketInfoMap m_pendingWimaxPackets;
  AnimUidPacketInfoMap m_pendingLtePackets;
  AnimUidPacketInfoMap m_pendingCsmaPackets;
  AnimUidPacketInfoMap m_pendingUanPackets;
  std::map<uint32_t, Vector> m_nodeLocation;
  std::map <std::string, uint32_t> m_macToNodeIdMap;
  std::map <std::string, uint32_t> m_ipv4ToNodeIdMap;
  NodeColorsMap m_nodeColors;
  NodeDescriptionsMap m_nodeDescriptions;
  LinkPropertiesMap m_linkProperties;
  EnergyFractionMap m_nodeEnergyFraction;
  uint64_t m_currentPktCount;
  std::vector <Ipv4RouteTrackElement> m_ipv4RouteTrackElements;
  std::map <uint32_t, NodeSize> m_nodeSizes;
  std::vector <std::string> m_resources;
  std::vector <std::string> m_nodeCounters;

  NodeCounterMap64 m_nodeIpv4Drop;
  NodeCounterMap64 m_nodeIpv4Tx;
  NodeCounterMap64 m_nodeIpv4Rx;
  NodeCounterMap64 m_nodeQueueEnqueue;
  NodeCounterMap64 m_nodeQueueDequeue;
  NodeCounterMap64 m_nodeQueueDrop;
  NodeCounterMap64 m_nodeWifiMacTx;
  NodeCounterMap64 m_nodeWifiMacTxDrop;
  NodeCounterMap64 m_nodeWifiMacRx;
  NodeCounterMap64 m_nodeWifiMacRxDrop;
  NodeCounterMap64 m_nodeWifiPhyTxDrop;
  NodeCounterMap64 m_nodeWifiPhyRxDrop;

  const std::vector<std::string> GetElementsFromContext (const std::string& context) const;
  Ptr <Node> GetNodeFromContext (const std::string& context) const;
  Ptr <NetDevice> GetNetDeviceFromContext (std::string context);
  
  // ##### General #####
  void StartAnimation (bool restart = false);
  void SetOutputFile (const std::string& fn, bool routing = false);
  void StopAnimation (bool onlyAnimation = false);
  std::string CounterTypeToString (CounterType counterType);
  std::string GetPacketMetadata (Ptr<const Packet> p);
  void AddByteTag (uint64_t animUid, Ptr<const Packet> p);
  int WriteN (const char*, uint32_t, FILE * f);
  int WriteN (const std::string&, FILE * f);
  std::string GetMacAddress (Ptr <NetDevice> nd);
  std::string GetIpv4Address (Ptr <NetDevice> nd);
  std::string GetNetAnimVersion ();
  void MobilityAutoCheck ();
  bool IsPacketPending (uint64_t animUid, ProtocolType protocolType);
  void PurgePendingPackets (ProtocolType protocolType);
  AnimUidPacketInfoMap * ProtocolTypeToPendingPackets (ProtocolType protocolType);
  void AddPendingPacket (ProtocolType protocolType, uint64_t animUid, AnimPacketInfo pktInfo);
  uint64_t GetAnimUidFromPacket (Ptr <const Packet>);
  void AddToIpv4AddressNodeIdTable (std::string, uint32_t);
  bool IsInTimeWindow ();
  void CheckMaxPktsPerTraceFile ();

  void TrackWifiPhyCounters ();
  void TrackWifiMacCounters ();
  void TrackIpv4L3ProtocolCounters ();
  void TrackQueueCounters ();
  // ##### Routing #####
  void TrackIpv4Route ();
  void TrackIpv4RoutePaths ();
  std::string GetIpv4RoutingTable (Ptr <Node> n);
  void RecursiveIpv4RoutePathSearch (std::string fromIpv4, std::string toIpv4, Ipv4RoutePathElements &);
  void WriteRoutePath (uint32_t nodeId, std::string destination, Ipv4RoutePathElements rpElements);


  // ##### Trace #####
  void EnqueueTrace (std::string context,
                     Ptr<const Packet>);
  void DequeueTrace (std::string context,
                     Ptr<const Packet>);
  void QueueDropTrace (std::string context,
                     Ptr<const Packet>);
  void Ipv4TxTrace (std::string context,
                    Ptr<const Packet>, Ptr<Ipv4>,  
                    uint32_t);
  void Ipv4RxTrace (std::string context,
                    Ptr<const Packet>, Ptr<Ipv4>,
                    uint32_t);
  void Ipv4DropTrace (std::string context,
                      const Ipv4Header &, 
                      Ptr<const Packet>, 
                      Ipv4L3Protocol::DropReason, Ptr<Ipv4>, 
                      uint32_t);

  void WifiMacTxTrace (std::string context,
                       Ptr<const Packet>);
  void WifiMacTxDropTrace (std::string context,
                           Ptr<const Packet>);
  void WifiMacRxTrace (std::string context,
                       Ptr<const Packet>);
  void WifiMacRxDropTrace (std::string context,
                           Ptr<const Packet>);
  void WifiPhyTxDropTrace (std::string context,
                       Ptr<const Packet>);
  void WifiPhyRxDropTrace (std::string context,
                       Ptr<const Packet>);
  void DevTxTrace (std::string context,
                   Ptr<const Packet> p,
                   Ptr<NetDevice> tx,
                   Ptr<NetDevice> rx,
                   Time txTime,
                   Time rxTime);
  void WifiPhyTxBeginTrace (std::string context,
                            Ptr<const Packet> p);
  void WifiPhyRxBeginTrace (std::string context,
                            Ptr<const Packet> p);
  void WimaxTxTrace (std::string context,
                     Ptr<const Packet> p,
		     const Mac48Address &);
  void WimaxRxTrace (std::string context,
                     Ptr<const Packet> p,
                     const Mac48Address &);
  void CsmaPhyTxBeginTrace (std::string context,
                            Ptr<const Packet> p);
  void CsmaPhyTxEndTrace (std::string context,
                            Ptr<const Packet> p);
  void CsmaPhyRxEndTrace (std::string context,
                          Ptr<const Packet> p);
  void CsmaMacRxTrace (std::string context,
                       Ptr<const Packet> p);
  void LteTxTrace (std::string context,
                      Ptr<const Packet> p,
                      const Mac48Address &);
  void LteRxTrace (std::string context,
                      Ptr<const Packet> p,
                      const Mac48Address &);
  void LteSpectrumPhyTxStart (std::string context,
                      Ptr<const PacketBurst> pb);
  void LteSpectrumPhyRxStart (std::string context,
                      Ptr<const PacketBurst> pb);
  void UanPhyGenTxTrace (std::string context,
                         Ptr<const Packet>);
  void UanPhyGenRxTrace (std::string context,
                         Ptr<const Packet>);
  void RemainingEnergyTrace (std::string context, double previousEnergy, double currentEnergy);

  
  void ConnectCallbacks ();
  void ConnectLte ();
  void ConnectLteUe (Ptr <Node> n, Ptr <LteUeNetDevice> nd, uint32_t devIndex);
  void ConnectLteEnb (Ptr <Node> n, Ptr <LteEnbNetDevice> nd, uint32_t devIndex);


  // ##### Mobility #####
  Vector GetPosition (Ptr <Node> n);
  Vector UpdatePosition (Ptr <Node> n);
  Vector UpdatePosition (Ptr <Node> n, Vector v);
  bool NodeHasMoved (Ptr <Node> n, Vector newLocation);
  std::vector < Ptr <Node> > GetMovedNodes ();
  void MobilityCourseChangeTrace (Ptr <const MobilityModel> mob);


  // ##### XML Helpers ##### 

  void WriteNonP2pLinkProperties (uint32_t id, std::string ipv4Address, std::string channelType);
  void WriteNodeUpdate (uint32_t nodeId);
  void OutputWirelessPacketTxInfo (Ptr<const Packet> p, AnimPacketInfo& pktInfo, uint64_t animUid);
  void OutputWirelessPacketRxInfo (Ptr<const Packet> p, AnimRxInfo pktrxInfo, uint64_t animUid);
  void OutputCsmaPacket (Ptr<const Packet> p, AnimPacketInfo& pktInfo, AnimRxInfo pktrxInfo);
  void WriteLinkProperties ();
  void WriteNodes ();
  void WriteNodeColors ();
  void WriteNodeSizes ();
  void WriteNodeEnergies ();
  void WriteXmlAnim (bool routing = false);
  void WriteXmlUpdateNodePosition (uint32_t nodeId, double x, double y);
  void WriteXmlUpdateNodeColor (uint32_t nodeId, uint8_t r, uint8_t g, uint8_t b);
  void WriteXmlUpdateNodeDescription (uint32_t nodeId);
  void WriteXmlUpdateNodeSize (uint32_t nodeId, double width, double height);
  void WriteXmlAddResource (uint32_t resourceId, std::string resourcePath);
  void WriteXmlAddNodeCounter (uint32_t counterId, std::string counterName, CounterType counterType);
  void WriteXmlUpdateNodeImage (uint32_t nodeId, uint32_t resourceId);
  void WriteXmlUpdateNodeCounter (uint32_t counterId, uint32_t nodeId, double value);
  void WriteXmlNode (uint32_t id, uint32_t sysId, double locX, double locY);
  void WriteXmlLink (uint32_t fromId, uint32_t toLp, uint32_t toId);
  void WriteXmlUpdateLink (uint32_t fromId, uint32_t toId, std::string);
  void WriteXmlP (std::string pktType, 
                                 uint32_t fId, 
                                 double fbTx, 
                                 double lbTx, 
                                 uint32_t tId, 
                                 double fbRx, 
                                 double lbRx,
                                 std::string metaInfo = ""); 
  void WriteXmlP (uint64_t animUid, std::string pktType, uint32_t fId, double fbTx, double lbTx);
  void WriteXmlPRef (uint64_t animUid, uint32_t fId, double fbTx, double lbTx, std::string metaInfo = "");
  void WriteXmlClose (std::string name, bool routing = false);
  void WriteXmlNonP2pLinkProperties (uint32_t id, std::string ipv4Address, std::string channelType);
  void WriteXmlRouting (uint32_t id, std::string routingInfo);
  void WriteXmlRp (uint32_t nodeId, std::string destination, Ipv4RoutePathElements rpElements);
  void WriteXmlUpdateBackground (std::string fileName, double x, double y, double scaleX, double scaleY, double opacity);

};




/**
 * \ingroup netanim
 *
 * \brief Byte tag using by Anim to uniquely identify packets
 *
 * When Anim receives a Tx Notification we tag the packet with a unique global uint64_t identifier
 * before recording Tx information
 * When Anim receives Rx notifications the tag is used to retrieve Tx information recorded earlier 
 * 
 */

class AnimByteTag : public Tag
{
public:

  /**
   * \brief Get Type Id
   * \returns Type Id
   *
   */
  static TypeId GetTypeId (void);
  
  /**
   * \brief Get Instance Type Id
   * \returns Type Id
   *
   */
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * \brief Get Serialized Size
   * \returns Serialized Size (i.e size of uint64_t)
   *
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serialize function
   * \param i Tag Buffer
   *
   */
  virtual void Serialize (TagBuffer i) const;

  /**
   * \brief Deserialize function
   * \param i Tag Buffer
   *
   */
  virtual void Deserialize (TagBuffer i);

  /**
   * \brief Print tag info
   * \param os Reference of ostream object
   *
   */
  virtual void Print (std::ostream &os) const;

  /**
   * \brief Set global Uid in tag
   * \param AnimUid global Uid
   *
   */
  void Set (uint64_t AnimUid);

  /**
   * \brief Get Uid in tag
   * \returns Uid in tag
   *
   */
  uint64_t Get (void) const;

private:
  uint64_t m_AnimUid;
};



}
#endif

