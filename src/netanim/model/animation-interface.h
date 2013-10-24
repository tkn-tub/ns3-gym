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
#include "ns3/animation-interface-helper.h"
#include "ns3/mac48-address.h"
#include "ns3/lte-ue-net-device.h"
#include "ns3/lte-enb-net-device.h"
#include "ns3/uan-phy-gen.h"
#include "ns3/rectangle.h"

namespace ns3 {

#define MAX_PKTS_PER_TRACE_FILE 100000
struct Rgb;
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

struct Ipv4RouteTrackElement {
  std::string destination;
  uint32_t fromNodeId;
};


typedef struct {
  uint32_t nodeId;
  std::string nextHop;
  
} Ipv4RoutePathElement;

/**
 * \defgroup netanim Netanim
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
   * \param maxPktsPerFile The maximum number of packets per trace file.
	    AnimationInterface will create trace files with the following 
            filenames : filename, filename-1, filename-2..., filename-N
	    where each file contains packet info for 'maxPktPerFile' number of packets
   * \param usingXML Set to true if XML output traces are required
   *
   */
  AnimationInterface (const std::string filename, 
	uint64_t maxPktsPerFile = MAX_PKTS_PER_TRACE_FILE, 
	bool usingXML = true);

  /**
   * \brief Destructor for the animator interface.
   *
   */
  ~AnimationInterface ();

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
  AnimationInterface & EnableIpv4RouteTracking (std::string fileName, Time startTime, Time stopTime, Time pollInterval = Seconds(5));

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
  AnimationInterface & EnableIpv4RouteTracking (std::string fileName, Time startTime, Time stopTime, NodeContainer nc, Time pollInterval = Seconds(5));

  /**
   * \brief Check if AnimationInterface is initialized
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
   * \brief Set mobility poll interval:WARNING: setting a low interval can 
   * cause slowness
   *
   * \param t Time interval between fetching mobility/position information
   * Default: 0.25s
   *
   */
  void SetMobilityPollInterval (Time t);

  /**
   * \brief Set random position if a Mobility Model does not exists for the node
   *
   * \param setRandPos True if a random position can be set for a node without a
   * Mobililty model
   *
   */
  void SetRandomPosition (bool setRandPos);

  /**
   * \brief typedef for WriteCallBack used for listening to AnimationInterface
   * write messages
   * 
   */
  typedef void (*AnimWriteCallback) (const char * str);

  /**
   * \brief Set a callback function to listen to AnimationInterface write events
   *
   * \param cb Address of callback function
   *
   */
  void SetAnimWriteCallback (AnimWriteCallback cb);

  /**
   * \brief Reset the write callback function
   *
   */
  void ResetAnimWriteCallback ();

  /**
   * \brief Helper function to set Constant Position for a given node
   * \param n Ptr to the node
   * \param x X co-ordinate of the node
   * \param y Y co-ordinate of the node
   * \param z Z co-ordinate of the node
   *
   */
  static void SetConstantPosition (Ptr <Node> n, double x, double y, double z=0);

  /**
   * \brief Helper function to set the topology boundary rectangle
   * \param minX X value of the lower left corner
   * \param minY Y value of the lower left corner
   * \param maxX X value of the upper right corner
   * \param maxX Y value of the upper right corner
   *
   */
  static void SetBoundary (double minX, double minY, double maxX, double maxY);

  /**
   * \brief Helper function to set a brief description for a given node
   * \param n Ptr to the node
   * \param descr A string to briefly describe the node
   *
   */
  static void SetNodeDescription (Ptr <Node> n, std::string descr);

  /**
   * \brief Helper function to update the description for a given node
   * \param n Ptr to the node
   * \param descr A string to briefly describe the node
   *
   */
  void UpdateNodeDescription (Ptr <Node> n, std::string descr);

  /**
   * \brief Helper function to update the description for a given node
   * \param nodeId Id of the node
   * \param descr A string to briefly describe the node
   *
   */
  void UpdateNodeDescription (uint32_t nodeId, std::string descr);

 /**
  * \brief Helper function to show/hide a node
  * \param nodeId Id of the node
  * \param show Set to true to show node, set to false to hide
  *
  */
  void ShowNode (uint32_t nodeId, bool show = true);

 /**
  * \brief Helper function to show/hide a node
  * \param n Ptr to the node
  * \param show Set to true to show node, set to false to hide
  *
  */
  void ShowNode (Ptr <Node> n, bool show = true);

  /**
   * \brief Helper function to set a brief description for nodes in a Node Container
   * \param nc NodeContainer containing the nodes
   * \param descr A string to briefly describe the nodes
   *
   */
  static void SetNodeDescription (NodeContainer nc, std::string descr);

  /**
   * \brief Helper function to set the node color
   * \param n Ptr to the node
   * \param r Red component value (0-255)
   * \param g Green component value (0-255)
   * \param b Blue component value (0-255)
   *
   */
  static void SetNodeColor (Ptr <Node> n, uint8_t r, uint8_t g, uint8_t b);


  /**
   * \brief Helper function to update the node color
   * \param n Ptr to the node
   * \param r Red component value (0-255)
   * \param g Green component value (0-255)
   * \param b Blue component value (0-255)
   *
   */
  void UpdateNodeColor (Ptr <Node> n, uint8_t r, uint8_t g, uint8_t b);

  /**
   * \brief Helper function to update the node color
   * \param nodeId Id of the node
   * \param r Red component value (0-255)
   * \param g Green component value (0-255)
   * \param b Blue component value (0-255)
   *
   */
  void UpdateNodeColor (uint32_t nodeId, uint8_t r, uint8_t g, uint8_t b);


  /**
   * \brief Helper function to set the color of nodes in a container
   * \param n Ptr to the node
   * \param r Red component value (0-255)
   * \param g Green component value (0-255)
   * \param b Blue component value (0-255)
   *
   */
  static void SetNodeColor (NodeContainer nc, uint8_t r, uint8_t g, uint8_t b);

  /**
   * \brief Helper function to set the description for a link
   * \param fromNode Node Id of the "from Node" of the p2p link
   * \param toNode Node Id of the "to Node" of the p2p link
   * \param linkDescription Description of the link such as link bandwidth
   * \param fromNodeDescription Description at the "from Node" end such as IP address
   * \param toNodeDescription Description at the "to Node" end such as Ip address
   *
   */
  static void SetLinkDescription (uint32_t fromNode, uint32_t toNode, 
                                  std::string linkDescription,
                                  std::string fromNodeDescription = "",
                                  std::string toNodeDescription = "");

  /**
   * \brief Helper function to set the description for a link
   * \param fromNode Ptr to the "from Node" of the p2p link
   * \param toNode Ptr the "to Node" of the p2p link
   * \param linkDescription Description of the link such as link bandwidth
   * \param fromNodeDescription Description at the "from Node" end such as IP address
   * \param toNodeDescription Description at the "to Node" end such as Ip address
   *
   */
  static void SetLinkDescription (Ptr <Node> fromNode, Ptr <Node> toNode,
                                  std::string linkDescription,
                                  std::string fromNodeDescription = "",
                                  std::string toNodeDescription = "");


  /**
   * \brief Helper function to update the description for a link
   * \param fromNode Node Id of the "from Node" of the p2p link
   * \param toNode Node Id of the "to Node" of the p2p link
   * \param linkDescription Description of the link such as link bandwidth
   *
   */
  void UpdateLinkDescription (uint32_t fromNode, uint32_t toNode,
                              std::string linkDescription);

  /**
   * \brief Helper function to update the description for a link
   * \param fromNode Ptr to the "from Node" of the p2p link
   * \param toNode Ptr to the "to Node" of the p2p link
   * \param linkDescription Description of the link such as link bandwidth
   *
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
   * \returns true if AnimationInterface was started
   *
   */
  bool IsStarted (void);

  /**
   *
   * \brief Enable Packet metadata
   * \param enable if true enables writing the packet metadata to the XML trace file
   *        if false disables writing the packet metadata
   */
  void EnablePacketMetadata (bool enable);

  /**
   *
   * \brief Get trace file packet count (This used only for testing)
   *
   * returns Number of packets recorded in the current trace file
   *
   */
  uint64_t GetTracePktCount ();

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  int64_t AssignStreams (int64_t stream);

private:
  FILE * m_f; // File handle for output (0 if none)
  FILE * m_routingF; // File handle for routing table output (0 if None);
  // Write specified amount of data to the specified handle
  int WriteN (const char*, uint32_t, FILE * f);
  bool m_xml;      // True if xml format desired
  Time m_mobilityPollInterval;
  std::string m_outputFileName;
  bool m_outputFileSet;
  uint64_t gAnimUid ;    // Packet unique identifier used by Animtion
  bool m_randomPosition;
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
  
  void TrackIpv4Route ();
  void TrackIpv4RoutePaths ();
  std::string GetIpv4RoutingTable (Ptr <Node> n);

  /**
   * \brief Specify that animation commands are to be written
   * to the specified output file.
   *
   * This call is used to write the animation information to a text
   * file that can later be used as input to the network animator tool.
   *
   * \param fn The name of the output file.
   * \returns true if successful open.
   *
   */
  bool SetOutputFile (const std::string& fn);
  bool SetRoutingOutputFile (const std::string& fn);

  /**
   * \brief Specify that animation commands are to be written
   * in XML format.
   *
   * \returns none
   *
   */
  void SetXMLOutput ();

  /**
   * \brief Writes the topology information and sets up the appropriate
   *  animation packet tx callback
   *
   * Writes the topology information to the appropriate output, depending
   * on prior calls to SetOutputFile, SetServerPort, or SetInternalAnimation.
   * Then creates the callbacks needed for the animator to start processing
   * packets.
   *
   * \param restart True when restarting animation
   */
  void StartAnimation (bool restart = false);

  /**
   * \brief Closes the interface to the animator.
   *
   */
  void StopAnimation (bool onlyAnimation = false);

  void DevTxTrace (std::string context,
                   Ptr<const Packet> p,
                   Ptr<NetDevice> tx,
                   Ptr<NetDevice> rx,
                   Time txTime,
                   Time rxTime);
  void WifiPhyTxBeginTrace (std::string context,
                            Ptr<const Packet> p);
  void WifiPhyTxEndTrace (std::string context,
                          Ptr<const Packet> p);
  void WifiPhyTxDropTrace (std::string context,
                           Ptr<const Packet> p);
  void WifiPhyRxBeginTrace (std::string context,
                            Ptr<const Packet> p);
  void WifiPhyRxEndTrace (std::string context,
                          Ptr<const Packet> p);
  void WifiMacRxTrace (std::string context,
                       Ptr<const Packet> p);
  void WifiPhyRxDropTrace (std::string context,
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

  void MobilityCourseChangeTrace (Ptr <const MobilityModel> mob);

  // Write a string to the specified handle;
  int  WriteN (const std::string&, FILE * f);

  void OutputWirelessPacket (Ptr<const Packet> p, AnimPacketInfo& pktInfo, AnimRxInfo pktrxInfo);
  void OutputCsmaPacket (Ptr<const Packet> p, AnimPacketInfo& pktInfo, AnimRxInfo pktrxInfo);
  void MobilityAutoCheck ();
  

  std::map<uint64_t, AnimPacketInfo> m_pendingWifiPackets;
  void AddPendingWifiPacket (uint64_t AnimUid, AnimPacketInfo&);
  bool WifiPacketIsPending (uint64_t AnimUid); 

  std::map<uint64_t, AnimPacketInfo> m_pendingWimaxPackets;
  void AddPendingWimaxPacket (uint64_t AnimUid, AnimPacketInfo&);
  bool WimaxPacketIsPending (uint64_t AnimUid); 

  std::map<uint64_t, AnimPacketInfo> m_pendingLtePackets;
  void AddPendingLtePacket (uint64_t AnimUid, AnimPacketInfo&);
  bool LtePacketIsPending (uint64_t AnimUid);

  std::map<uint64_t, AnimPacketInfo> m_pendingCsmaPackets;
  void AddPendingCsmaPacket (uint64_t AnimUid, AnimPacketInfo&);
  bool CsmaPacketIsPending (uint64_t AnimUid);

  std::map<uint64_t, AnimPacketInfo> m_pendingUanPackets;
  void AddPendingUanPacket (uint64_t AnimUid, AnimPacketInfo&);
  bool UanPacketIsPending (uint64_t AnimUid);

  uint64_t GetAnimUidFromPacket (Ptr <const Packet>);

  std::map<uint32_t, Vector> m_nodeLocation;
  Vector GetPosition (Ptr <Node> n);
  Vector UpdatePosition (Ptr <Node> n);
  Vector UpdatePosition (Ptr <Node> n, Vector v);
  void WriteDummyPacket ();
  bool NodeHasMoved (Ptr <Node> n, Vector newLocation);

  void PurgePendingWifi ();
  void PurgePendingWimax ();
  void PurgePendingLte ();
  void PurgePendingCsma ();

  // Recalculate topology bounds
  void RecalcTopoBounds (Vector v);
  std::vector < Ptr <Node> > RecalcTopoBounds ();

  void ConnectCallbacks ();
  void ConnectLte ();
  void ConnectLteUe (Ptr <Node> n, Ptr <LteUeNetDevice> nd, uint32_t devIndex);
  void ConnectLteEnb (Ptr <Node> n, Ptr <LteEnbNetDevice> nd, uint32_t devIndex);

  
  std::map <std::string, uint32_t> m_macToNodeIdMap;
  std::map <std::string, uint32_t> m_ipv4ToNodeIdMap;
  void AddToIpv4AddressNodeIdTable (std::string, uint32_t);
  std::vector <Ipv4RouteTrackElement> m_ipv4RouteTrackElements;
  typedef std::vector <Ipv4RoutePathElement> Ipv4RoutePathElements;
  void RecursiveIpv4RoutePathSearch (std::string fromIpv4, std::string toIpv4, Ipv4RoutePathElements &);
  void WriteRoutePath (uint32_t nodeId, std::string destination, Ipv4RoutePathElements rpElements);
  bool IsInTimeWindow ();

  // Path helper
  std::vector<std::string> GetElementsFromContext (std::string context);
  Ptr <NetDevice> GetNetDeviceFromContext (std::string context);

  static std::map <uint32_t, Rgb> nodeColors;
  static std::map <uint32_t, std::string> nodeDescriptions;
  static std::map <P2pLinkNodeIdPair, LinkProperties, LinkPairCompare> linkProperties;
  uint64_t m_currentPktCount;

  void StartNewTraceFile();

  std::string GetMacAddress (Ptr <NetDevice> nd);
  std::string GetIpv4Address (Ptr <NetDevice> nd);
  void WriteNonP2pLinkProperties (uint32_t id, std::string ipv4Address, std::string channelType);

  std::string GetNetAnimVersion ();

  // XML helpers
  std::string GetPreamble (void);
  // Topology element dimensions
  double m_topoMinX;
  double m_topoMinY;
  double m_topoMaxX;
  double m_topoMaxY;
  static Rectangle * userBoundary;

  std::string GetPacketMetadata (Ptr<const Packet> p);

  std::string GetXMLOpen_anim (uint32_t lp);
  std::string GetXMLOpen_topology (double minX, double minY, double maxX, double maxY);
  std::string GetXMLOpenClose_node (uint32_t lp, uint32_t id, double locX, double locY);
  std::string GetXMLOpenClose_node (uint32_t lp, uint32_t id, double locX, double locY, struct Rgb rgb);
  std::string GetXMLOpenClose_nodeupdate (uint32_t id, bool visible = true);
  std::string GetXMLOpenClose_link (uint32_t fromLp, uint32_t fromId, uint32_t toLp, uint32_t toId);
  std::string GetXMLOpenClose_linkupdate (uint32_t fromId, uint32_t toId, std::string);
  std::string GetXMLOpen_packet (uint32_t fromLp, uint32_t fromId, double fbTx, double lbTx, std::string auxInfo = "");
  std::string GetXMLOpenClose_p (std::string pktType, uint32_t fId, double fbTx, double lbTx, uint32_t tId, double fbRx, double lbRx,
                                 std::string metaInfo = "", std::string auxInfo = "");
  std::string GetXMLOpenClose_rx (uint32_t toLp, uint32_t toId, double fbRx, double lbRx);
  std::string GetXMLOpen_wpacket (uint32_t fromLp, uint32_t fromId, double fbTx, double lbTx, double range);
  std::string GetXMLClose (std::string name) {return "</" + name + ">\n"; }
  std::string GetXMLOpenClose_meta (std::string metaInfo);
  std::string GetXMLOpenClose_NonP2pLinkProperties (uint32_t id, std::string ipv4Address, std::string channelType);
  std::string GetXMLOpenClose_routing (uint32_t id, std::string routingInfo);
  std::string GetXMLOpenClose_rp (uint32_t nodeId, std::string destination, Ipv4RoutePathElements rpElements);



  /// Provides uniform random variables.
  Ptr<UniformRandomVariable> m_uniformRandomVariable;  
};

/**
 * \ingroup netanim
 * \brief A structure to store red, blue and green components for entities such as nodes
 *
 */
struct Rgb 
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
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

