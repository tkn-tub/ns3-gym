/**
 * \ingroup devices
 * \defgroup Wifi Wifi Models
 *
 * \section WifiModelOverview Wifi Models Overview
 *
 * The set of 802.11 models provided in ns-3 attempts to provide
 * an accurate MAC-level implementation of the 802.11 specification
 * and to provide a not-so-slow PHY-level model of the 802.11a
 * and 802.11b specifications.
 *
 * The current implementation provides roughly 4 levels of models:
 *   - the PHY layer models
 *   - the so-called MAC low models: they implement DCF and EDCAF
 *   - the so-called MAC high models: they implement the MAC-level
 *     beacon generation, probing, and association state machines.
 *   - a set of Rate control algorithms used by the MAC low models.
 *
 * There are presently three MAC high models, corresponding to the
 * three primary Wi-Fi topological elements - Access Point (AP)
 * (implemented in ns3::ApWifiMac), non-AP Station (STA) (implemented
 * in ns3::StaWifiMac), and STA in an Independent Basic Service Set
 * (IBSS - also commonly referred to as an ad hoc network. Implemented
 * in ns3::AdhocWifiMac).
 *
 *
 * The simplest of these is ns3::AdhocWifiMac, which implements a
 * Wi-Fi MAC that does not perform any kind of beacon generation,
 * probing, or association. The ns3::StaWifiMac class implements an
 * active probing and association state machine that handles automatic
 * re-association whenever too many beacons are missed. Finally,
 * ns3::ApWifiMac implements an AP that generates periodic beacons,
 * and that accepts every attempt to associate.
 *
 * These three MAC high models share a common parent in
 * ns3::RegularWifiMac, which exposes, among other MAC configuration,
 * an attribute (QosSupported) that allows configuration of
 * 802.11e/WMM-style QoS support. With QoS-enabled MAC models it is
 * possible to work with traffic belonging to four different Access
 * Categories (ACs): AC_VO for voice traffic, AC_VI for video traffic,
 * AC_BE for best-effort traffic and AC_BK for background traffic.  In
 * order for the MAC to determine the appropriate AC for an MSDU,
 * packets forwarded down to these MAC layers should be marked using
 * ns3::QosTag in order to set a TID (traffic id) for that packet
 * otherwise it will be considered belonging to AC_BE.
 *
 * How TIDs are mapped to access classes are shown in the table below.
 *
 * <table border=1>
 * <tr>
 *   <td><b> TID </b></td>
 *   <td><b> Access Category </b></td>
 * </tr>
 * <tr>
 *   <td> 7 </td>
 *   <td> AC_VO </td>
 * </tr>
 * <tr>
 *   <td> 6 </td>
 *   <td> AC_VO </td>
 * </tr>
 * <tr>
 *   <td> 5 </td>
 *   <td> AC_VI </td>
 * </tr>
 * <tr>
 *   <td> 4 </td>
 *   <td> AC_VI </td>
 * </tr>
 * <tr>
 *   <td> 3 </td>
 *   <td> AC_BE </td>
 * </tr>
 * <tr>
 *   <td> 0 </td>
 *   <td> AC_BE </td>
 * </tr>
 * <tr>
 *   <td> 2 </td>
 *   <td> AC_BK </td>
 * </tr>
 * <tr>
 *   <td> 1 </td>
 *   <td> AC_BK </td>
 * </tr>
 * </table>
 *
 * The MAC low layer is split in 3 components:
 *   - ns3::MacLow which takes care of RTS/CTS/DATA/ACK transactions.
 *   - ns3::DcfManager and ns3::DcfState which implements the DCF function.
 *
 *   - ns3::DcaTxop and ns3::EdcaTxopN which handle the packet queue,
 *     packet fragmentation, and packet retransmissions if they are
 *     needed.  The ns3::DcaTxop object is used by high MACs that are
 *     not QoS-enabled, and for transmission of frames (e.g., of type
 *     Management) that the standard says should access the medium
 *     using the DCF. ns3::EdcaTxopN is used by QoS-enabled high MACs
 *     and also performs QoS operations like 802.11n-style MSDU
 *     aggregation.
 *
 * The PHY layer implements a single 802.11a model in the ns3::WifiPhy class: the
 * physical layer model implemented there is described fully in a paper titled
 * "Yet Another Network Simulator", available at: http://cutebugs.net/files/wns2-yans.pdf 
 * and recently extended to cover 802.11b physical layer.
 * 
 * The Wifi Model also provides a set of Rate control algorithms:
 *   - ns3::ArfWifiManager
 *   - ns3::AarfWifiManager
 *   - ns3::IdealWifiManager
 *   - ns3::CrWifiManager
 *   - ns3::OnoeWifiManager
 *   - ns3::AmrrWifiManager
 *   - ns3::CaraWifiManager
 *   - ns3::AarfcdWifiManager
 *
 * \section WifiTracingModel Wifi Tracing Model
 *
 * Like all ns-3 devices, the Wifi Model provides a number of trace sources.
 * These trace sources can be hooked using your own custom trace code, or you
 * can use our helper functions to arrange for tracing to be enabled on devices
 * you specify.
 *
 * \subsection WifiTracingModelUpperHooks Upper-Level (MAC) Hooks
 *
 * From the point of view of tracing in the net device, there are several 
 * interesting points to insert trace hooks.  The first is at the interface
 * between the device and higher layers.  We provide trace hooks at this point
 * in packet flow, which corresponds to a transition from the network to data 
 * link layer, and call them collectively the device MAC hooks.
 *
 * The first trace hook is called "Rx" and is fired using the 
 * ns3::WifiNetDevice::m_rxLogger trace hook.  The perspective here is looking
 * down into the WifiNetDevice so a receive indicates a packet being sent up
 * from the channel to be forwarded up to higher layers.
 *
 * The second trace hook is called "Tx" and is fired using the 
 * ns3::WifiNetDevice::m_txLogger trace hook.  This trace hook indicates a 
 * packet has been sent from higher layers down to the net device for 
 * transmission onto the network.
 *
 * \subsection WifiTracingModelLowerHooks Low-Level (PHY) Hooks
 *
 * Another interesting place to insert trace hooks is in the state machine
 * that is driving the actual device transmission and reception logic.  We 
 * provide the following hooks to instrument the lower levels of the device.
 *
 * First, we provide a trace hook to indicate state changes.  This trace
 * source is called "State" and is fired using the 
 * ns3::WifiPhyStateHelper::m_stateLogger trace source.
 *
 * We also provide a trace hook to indicate the successful reception of a
 * packet from the channel.  This trace source is called "RxOk" and is 
 * accessed using the ns3::WifiPhyStateHelper::m_rxOkTrace trace source.
 *
 * There also exists a trace hook to indicate an unsuccessful reception of a
 * packet from the channel.  This trace source is called "RxError" and is 
 * accessed using the ns3::WifiPhyStateHelper::m_rxErrorTrace trace source.
 *
 * There is a trace hook to indicate that transmission of a packet is 
 * starting onto the channel.  This trace source is called "Tx" (don't 
 * confuse it with the higher layer "Tx" hook) and is 
 * fired using the ns3::WifiPhyStateHelper::m_txTrace trace source.
 *
 * \subsection WifiTracingModelRemoteHooks Remote Station Hooks
 *
 * We provide access to changes in the the per-remote-station RTS counter 
 * through the "Ssrc" trace source which is fired using the 
 *  ns3::WifiRemoteStation::m_ssrc trace hook.
 * 
 * Finally, we provide access to the per-remote-station SLRC counter that
 * indications the number of retransmissions of data.  Changes to this 
 * counter are traced using the ns3::WifiRemoteStation::m_slrc source.
 *
 * \subsection wifil2stack Layer 2 Stack Overview
 * \image html WifiArchitecture.png "Overview of the Wifi L2 sublayers traversed for transmitting and receiving a packet"
 */
