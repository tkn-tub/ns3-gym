/**
 * \ingroup devices
 * \defgroup CsmaModel CSMA Model
 *
 * \section CsmaModelOverview CSMA Model Overview
 *
 * The ns-3 CSMA device models a simple bus network in the spirit of Ethernet.
 * Although it does not model any real physical network you could ever build 
 * or buy, it does provide some very useful functionality.
 *
 * Typically when one thinks of a bus network Ethernet or IEEE 802.3 comes to
 * mind.  Ethernet uses CSMA/CD (Carrier Sense Multiple Access with Collision
 * Detection with exponentially increasing backoff to contend for the shared 
 * transmission medium.  The ns-3 CSMA device models only a portion of this 
 * process, using the nature of the globally available channel to provide 
 * instantaneous (faster than light) carrier sense and priority-based 
 * collision "avoidance."  Collisions in the sense of Ethernet never happen and
 * so the ns-3 CSMA device does not model collision detection, nor will any
 * transmission in progress be "jammed."
 *
 * \section CsmaLayerModel CSMA Layer Model
 *
 * There are a number of conventions in use for describing layered 
 * communications architectures in the literature and in textbooks.  The most
 * common layering  model is the ISO seven layer reference model.  In this view
 * the CsmaNetDevice and CsmaChannel pair occupies the lowest two 
 * layers -- at the physical (layer one), and data link (layer two) positions.
 * Another important reference model is that specified by RFC 1122, 
 * "Requirements for Internet Hosts -- Communication Layers."  In this view the
 * CsmaNetDevice and CsmaChannel pair occupies the lowest layer -- 
 * the link layer.  There is also a seemingly endless litany of alternative 
 * descriptions found in textbooks and in the literature.  We adopt the naming
 * conventions used in the IEEE 802 standards which speak of LLC, MAC, MII
 * and PHY layering.  These acronyms are defined as:
 *
 * - LLC:  Logical Link Control;
 * - MAC:  Media Access Control;
 * - MII:  Media Independent Interface;
 * - PHY:  Physical Layer.
 * 
 * In this case the LLC and MAC are sublayers of the OSI data link layer and the 
 * MII and PHY are sublayers of the OSI physical layer.
 *
 * The "top" of the CSMA device defines the transition from the network layer
 * to the data link layer.  This transition is performed by higher layers by 
 * calling either CsmaNetDevice::Send or CsmaNetDevice::SendFrom.
 *
 * In contrast to the IEEE 802.3 standards, there is no precisely specified
 * PHY in the CSMA model in the sense of wire types, signals or pinouts.  The
 * "bottom" interface of the CsmaNetDevice can be thought of as as a kind
 * of Media Independent Interface (MII) as seen in the "Fast Ethernet" 
 * (IEEE 802.3u) specifications.  This MII interface fits into a corresponding
 * media independent interface on the CsmaChannel.  You will not find the
 * equivalent of a 10BASE-T or a 1000BASE-LX PHY.
 *
 * The CsmaNetDevice calls the CsmaChannel through a media independent
 * interface.  There is a method defined to tell the channel when to start 
 * "wiggling the wires" using the method CsmaChannel::TransmitStart, and 
 * a method to tell the channel when the transmission process is done and
 * the channel should begin propagating the last bit across the "wire":
 * CsmaChannel::TransmitEnd.
 *
 * When the TransmitEnd method is executed, the channel will model a single 
 * uniform signal propagation delay in the medium and deliver copes of the packet
 * to each of the devices attached to the packet via the 
 * CsmaNetDevice::Receive method.
 *
 * There is a "pin" in the device media independent interface corresponding to 
 * "COL" (collision).  The state of the channel may be sensed by calling 
 * CsmaChannel::GetState.  Each device will look at this "pin" before 
 * starting a send and will perform appropriate backoff operations if required.
 *
 * Properly received packets are forwarded up to higher levels from the 
 * CsmaNetDevice via a callback mechanism.  The callback function is
 * initialized by the higher layer (when the net device is attached) using
 * CsmaNetDevice::SetReceiveCallback and is invoked upon "proper"
 *  reception of a packet by the net device in order to forward the packet up
 * the protocol stack.
 *
 * \section CsmaChannelModel CSMA Channel Model
 *
 * The class CsmaChannel models the actual transmission medium.
 * There is no fixed limit for the number of devices connected to the channel.
 * The CsmaChannel models a data rate and a speed-of-light delay which can
 * be accessed via the attributes "DataRate" and "Delay" respectively.
 * The data rate provided to the channel is used to set the data rates
 * used by the transmitter sections of the CSMA devices connected to the 
 * channel.  There is no way to independently set data rates in the
 * devices.  Since the data rate is only used to calculate a delay time, there
 * is no limitation (other than by the data type holding the value) on the 
 * speed at which CSMA channels and devices can operate; and no restriction
 * based on any kind of PHY characteristics.
 *
 * The CsmaChannel has three states, IDLE, TRANSMITTING and PROPAGATING.
 * These three states are "seen" instantaneously by all devices on the channel.
 * By this we mean that if one device begins or ends a simulated transmission,
 * all devices on the channel are immediately aware of the change in state.
 * There is no time during which one device may see an IDLE channel while
 * another device physically further away in the collision domain may have 
 * begun transmitting with the associated signals not propagated.  Thus there
 * is no need for collision detection in the CsmaChannel model and it is
 * not implemented in any way.
 *
 * We do, as the name indicates, have a Carrier Sense aspect to the model.
 * Since the simulator is single threaded, access to the common channel will
 * be serialized by the simulator.  This provides a deterministic mechanism
 * for contending for the channel.  The channel is allocated (transitioned from
 * state IDLE to state TRANSMITTING) on a first-come first-served basis.  The
 * channel always goes through a three state process:
 *
 *  IDLE -> TRANSMITTING -> PROPAGATING -> IDLE
 *
 * The TRANSMITTING state models the time during which the source net device
 * is actually wiggling the signals on the wire.  The PROPAGATING state models
 * the time after the last bit was sent, when the signal is propagating down 
 * the wire to the "far end."  
 *
 * The transition to the TRANSMITTING state is  driven by a call to 
 * CsmaChannel::TransmitStart which is called by the net device that 
 * transmits the packet.  It is the responsibility of that device to end the
 * transmission with a call to CsmaChannel::TransmitEnd at the appropriate
 * simulation time that reflects the time elapsed to put all of the packet bits
 * on the wire.  When TransmitEnd is called, the channel schedules an event
 * corresponding to a single speed-of-light delay.  This delay applies to all
 * net devices on the channel identically.  You can think of a symmetrical hub
 * in which the packet bits propagate to a central location and then back out
 * equal length cables to the other devices on the channel.
 *
 * The CsmaChannel models a broadcast medium so the packet is delivered
 * to all of the devices on the channel (including the source) at the end of 
 * the propagation time.  It is the responsibility of the sending device to 
 * determine whether or not it receives a packet broadcast over the channel.
 *
 * The CsmaChannel provides following Attributes:
 *
 * - DataRate:      The bitrate for packet transmission on connected devices;
 * - Delay:       The speed of light transmission delay for the channel.
 *
 * \section CsmaNetDeviceModel CSMA Net Device Model
 *
 * The CSMA network device appears somewhat like an Ethernet device.  The
 * CsmaNetDevice provides following Attributes:
 *
 * - Address:           The Mac48Address of the device;
 * - SendEnable:        Enable packet transmission if true;
 * - ReceiveEnable:     Enable packet reception if true;
 * - EncapsulationMode: Type of link layer encapsulation to use;
 * - RxErrorModel:      The receive error model;
 * - TxQueue:           The trasmit queue used by the device;
 * - InterframeGap:     The optional time to wait between "frames";
 * - Rx:                A trace source for received packets;
 * - Drop:              A trace source for dropped packets.
 *
 * The CsmaNetDevice supports the assignment of a "receive error model."
 * This is an ErrorModel object that is used to simulate data corruption
 * on the link.
 *
 * Packets sent over the CsmaNetDevice are always routed through the 
 * transmit queue to provide a trace hook for packets sent out over the 
 * network.  This transmit queue can be set (via attribute) to model different
 * queueing strategies.
 *
 * Also configurable by attribute is the encapsulation method used by the
 * device.  Every packet gets an EthernetHeader that includes the 
 * destination and source MAC addresses, and a length/type field.  Every packet
 * also gets an EthernetTrailer which includes the FCS.  Data in the
 * packet may be encapsulated in different ways.  By default, or by setting
 * the "EncapsulationMode" attribute to "Llc", the encapsulation is by 
 * LLC SNAP.  In this case, a SNAP header is added that contains the EtherType
 * (IP or ARP).  The other implemented encapsulation modes are IP_ARP (set
 * "EncapsulationMode" to "IpArp") in which the length type of the Ethernet 
 * header receives the protocol number of the packet; or ETHERNET_V1 (set
 * "EncapsulationMode" to "EthernetV1") in which the length type of the 
 * Ethernet header receives the length of the packet.  A "Raw" encapsulation
 * mode is defined but not implemented -- use of the RAW mode results in an
 * assert firing.
 *
 * The CsmaNetDevice implements a random exponential backoff algorithm 
 * that is executed if the channel is determined to be busy (TRANSMITTING or
 * PROPAGATING) when the device wants to start propagating.  This results in a
 * random delay of up to pow (2, retries) - 1 microseconds before a retry is
 * attempted.  The default maximum number of retries is 1000.
 *
 * \section CsmaTracingModel CSMA Tracing Model
 *
 * Like all ns-3 devices, the CSMA Model provides a number of trace sources.
 * These trace sources can be hooked using your own custom trace code, or you
 * can use our helper functions to arrange for tracing to be enabled on devices
 * you specify.
 *
 * \subsection CsmaTracingModelUpperHooks Upper-Level (MAC) Hooks
 *
 * From the point of view of tracing in the net device, there are several 
 * interesting points to insert trace hooks.  A convention inherited from other
 * simulators is that packets destined for transmission onto attached networks
 * pass through a single "transmit queue" in the net device.  We provide trace 
 * hooks at this point in packet flow, which corresponds (abstractly) only to a 
 * transition from the network to data link layer, and call them collectively
 * the device MAC hooks.
 *
 * When a packet is sent to the CSMA net device for transmission it always 
 * passed through the transmit queue.  The transmit queue in the 
 * CsmaNetDevice inherits from Queue, and therefore inherits three 
 * trace sources:
 *
 * - An Enqueue operation source (see Queue::m_traceEnqueue);
 * - A Dequeue operation source (see Queue::m_traceDequeue);
 * - A Drop operation source (see Queue::m_traceDrop).
 *
 * The upper-level (MAC) trace hooks for the CsmaNetDevice are, in fact, 
 * exactly these three trace sources on the single transmit queue of the device.  
 *
 * The m_traceEnqueue event is triggered when a packet is placed on the transmit
 * queue.  This happens at the time that CsmaNetDevice::Send or 
 * CsmaNetDevice::SendFrom is called by a higher layer to queue a packet for 
 * transmission.
 *
 * The m_traceDequeue event is triggered when a packet is removed from the
 * transmit queue.  Dequeues from the transmit queue can happen in three 
 * situations:  1) If the underlying channel is idle when the 
 * CsmaNetDevice::Send or CsmaNetDevice::SendFrom is called, a packet
 * is dequeued from the transmit queue and immediately transmitted;  2) If the
 * underlying channel is idle, a packet may be dequeued and immediately 
 * transmitted in an internal TransmitCompleteEvent that functions much 
 * like a transmit complete interrupt service routine; or 3) from
 * the random exponential backoff handler if a timeout is detected.
 *
 * Case (3) implies that a packet is dequeued from the transmit queue if it is 
 * unable to be transmittted according to the backoff rules.  It is important 
 * to understand that this will appear as a Dequeued packet and it is easy to 
 * incorrectly assume that the packet was transmitted since it passed through
 * the transmit queue.  In fact, a packet is actually dropped by the net device
 * in this case.  The reason for this behavior is due to the definition of the 
 * Queue Drop event.  The m_traceDrop event is, by defintion, fired when a 
 * packet cannot be enqueued on the transmit queue becasue it is full.  This 
 * event only fires if the queue is full and we do not overload this event
 * to indicate that the CsmaChannel is "full."
 *
 * \subsection CsmaTracingModelUpperHooks Lower-Level (PHY) Hooks
 *
 * Similar to the upper level trace hooks, there are trace hooks available at
 * the lower levels of the net device.  We call these the PHY hooks.  These 
 * events fire from the device methods that talk directly to the CsmaChannel.

 * The trace source m_dropTrace is called to indicate a packet that is dropped
 * by the device.  This happens in two cases:  First, if the receive side of 
 * the net device is not enabled (see CsmaNetDevice::m_receiveEnable and the 
 * associated attribute "ReceiveEnable").
 *
 * The m_dropTrace is also used to indicate that a packet was discarded as 
 * corrupt if a receive error model is used (see 
 * CsmaNetDevice::m_receiveErrorModel and the associated attribute 
 * "ReceiveErrorModel").
 *
 * The other low-level trace source fires on reception of an accepted packet
 * (see CsmaNetDevice::m_rxTrace).  A packet is accepted if it is destined
 * for the broadcast address, a multicast address, or to the MAC address 
 * assigned to the net device.
 *
 * \section CsmaModelSummary CSMA Model Summary
 *
 * The ns3 CSMA model is a simplistic model of an Ethernet-like network.  It
 * supports a Carrier-Sense function and allows for Multiple Access to a 
 * shared medium.  It is not physical in the sense that the state of the 
 * medium is instantaneously shared among all devices.  This means that there
 * is no collision detection required in this model and none is implemented.
 * There will never be a "jam" of a packet already on the medium.  Access to 
 * the shared channel is on a first-come first-served basis as determined by 
 * the simulator scheduler.  If the channel is determined to be busy by looking
 * at the global state, a random exponential backoff is performed and a retry
 * is attempted.
 *
 * Ns-3 Attributes provide a mechanism for setting various parameters in the 
 * device and channel such as addresses, encapsulation modes and error model
 * selection.  Trace hooks are provided in the usual manner with a set of 
 * upper level hooks corresponding to a transmit queue and used in ASCII 
 * tracing; and also a set of lower level hooks used in pcap tracing.
 *
 * Although the ns-3 CsmaChannel and CsmaNetDevice does not model any kind of
 * network you could build or buy, it does provide us with some useful 
 * functionality.  You should, however, understand that it is explicitly not 
 * Ethernet or any flavor of IEEE 802.3 but an interesting subset.
 */
