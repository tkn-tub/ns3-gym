/**
 * \ingroup devices
 * \defgroup CSMA CSMA Model
 *
 * \section CSMA Model
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
 * \subsection CSMA Channel Model
 *
 * The class ns3::CsmaChannel models the actual transmission medium.
 * There is no fixed limit for the number of devices connected to the channel.
 * The ns3::CsmaChannel models a bitrate and a speed-of-light delay which can
 * be accessed via the attributes "BitRate" and "Delay" respectively.
 *
 * The ns3::CsmaChannel has three states, IDLE, TRANSMITTING and PROPAGATING.
 * These three states are "seen" instantaneously by all devices on the channel.
 * By this we mean that if one device begins or ends a simulated transmission,
 * all devices on the channel are immediately aware of the change in state.
 * There is no time during which one device may see an IDLE channel while
 * another device physically further away in the collision domain may have 
 * begun transmitting with the associated signals not propagated.  Thus there
 * is no need for collision detection in the ns3::CsmaChannel model and it is
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
 * ns3::CsmaChannel::TransmitStart which is called by the net device that 
 * transmits the packet.  It is the responsibility of that device to end the
 * transmission with a call to ns3::CsmaChannel::TransmitEnd at the appropriate
 * simulation time that reflects the time elapsed to put all of the packet bits
 * on the wire.  When TransmitEnd is called, the channel schedules an event
 * corresponding to a single speed-of-light delay.  This delay applies to all
 * net devices on the channel identically.  You can think of a symmetrical hub
 * in which the packet bits propagate to a central location and then back out
 * equal length cables to the other devices on the channel.
 *
 * The ns3::CsmaChannel models a broadcast medium so the packet is delivered
 * to all of the devices on the channel (including the source) at the end of 
 * the propagation time.
 *
 * \subsection CSMA Net Device Model
 *
 * The CSMA network device appears somewhat like an Ethernet device.  The
 * ns3::CsmaNetDevice provides following Attributes:
 *
 * - Address:           The ns3::Mac48Address of the device;
 * - DataRate:          The data rate of the device;
 * - SendEnable:        Enable packet transmission if true;
 * - ReceiveEnable:     Enable packet reception if true;
 * - EncapsulationMode: Type of link layer encapsulation to use;
 * - RxErrorModel:      The receive error model;
 * - TxQueue:           The trasmit queue used by the device;
 * - InterframeGap:     The optional time to wait between "frames";
 * - Rx:                A trace source for received packets;
 * - Drop:              A trace source for dropped packets.
 *
 * The ns3::CsmaNetDevice supports the assignment of a "receive error model."
 * This is an ns3::ErrorModel object that is used to simulate data corruption
 * on the link.
 *
 * Packets sent over the ns3::CsmaNetDevice are always routed through the 
 * transmit queue to provide a trace hook for packets sent out over the 
 * network.  This transmit queue can be set (via attribute) to model different
 * queueing strategies.
 *
 * Also configurable by attribute is the encapsulation method used by the
 * device.  Every packet gets an ns3::EthernetHeader that includes the 
 * destination and source MAC addresses, and a length/type field.  Every packet
 * also gets an ns3::EthernetTrailer which includes the FCS.  Data in the
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
 * The ns3::CsmaNetDevice implements a random exponential backoff algorithm 
 * that is executed if the channel is determined to be busy (TRANSMITTING or
 * PROPAGATING) when the device wants to start propagating.  This results in a
 * random delay of up to pow (2, retries) - 1 microseconds before a retry is
 * attempted.  The default maximum number of retries is 1000.
 *
 * \subsection CSMA Model Summary
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
 * selection.  Trace hooks are provided in the usual manner.
 *
 * Although the ns-3 CsmaChannel and CsmaNetDevice does not model any kind of
 * network you could build or buy, it does provide us with some useful 
 * functionality.  You should, however, understand that it is explicitly not 
 * Ethernet or IEEE 802.3 but an interesting subset.
 */
