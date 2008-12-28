/**
 * \ingroup devices
 * \defgroup PointToPointModel Point-to-Point Model
 *
 * \section PointToPointPointOverview Point-to-Point Model Overview
 *
 * The ns-3 point-to-point model is of a very simple point to point data link
 * connecting exactly two ns3::PointToPointNetDevice devices over an
 * ns3::PointToPointChannel.  This can be viewed as equivalent to a full
 * duplex RS-232 or RS-422 link with null modem and no handshaking.
 *
 * Data is encapsulated in the Point-to-Point Protocol (PPP -- RFC 1661),
 * however the Link Control Protocol (LCP) and associated state machine is 
 * not implemented.  The PPP link is assumed to be established and 
 * authenticated at all times.
 *
 * Data is not framed, therefore Address and Control fields will not be found.
 * Since the data is not framed, there is no need to provide Flag Sequence and
 * Control Escape octets, nor is a Frame Check Sequence appended.  All that is
 * required to implement non-framed PPP is to prepend the PPP protocol number
 * for IP Version 4 which is the sixteen-bit number 0x21 (see
 * http://www.iana.org/assignments/ppp-numbers).
 *
 * The PointToPointNetDevice provides following Attributes:
 *
 * - Address:       The ns3::Mac48Address of the device (if desired);
 * - DataRate:      The data rate (ns3::DataRate) of the device;
 * - TxQueue:       The trasmit queue (ns3::Queue) used by the device;
 * - InterframeGap: The optional ns3::Time to wait between "frames";
 * - Rx:            A trace source for received packets;
 * - Drop:          A trace source for dropped packets.
 *
 * The PointToPointNetDevice models a transmitter section that puts bits
 * on a corresponding channel "wire."  `The DataRate attribute specifies the
 * number of bits per second that the device will simulate sending over the 
 * channel.  In reality no bits are sent, but an event is scheduled for an
 * elapsed time consistent with the number of bits in each packet and the 
 * specified DataRate.  The implication here is that the receiving device
 * models a receiver section that can receive any any data rate.  Therefore
 * there is no need, nor way to set a receive data rate in this model.  By
 * setting the DataRate on the transmitter of both devices connected to a 
 * given PointToPointChannel one can model a symmetric channel; or by 
 * setting different DataRates one can model an asymmetric channel (e.g., 
 * ADSL).
 *
 * The PointToPointNetDevice supports the assignment of a "receive error 
 * model."  This is an ns3::ErrorModel object that is used to simulate data
 * corruption on the link.
 *
 * \section PointToPointChannelModel Point-to-Point Channel Model

 * The point to point net devices are connected via an 
 * ns3::PointToPointChannel.  This channel models two wires transmitting bits
 * at the data rate specified by the source net device.  There is no overhead
 * beyond the eight bits per byte of the packet sent.  That is, we do not 
 * model Flag Sequences, Frame Check Sequences nor do we "escape" any data.
 *
 * The PointToPointNetChannel provides following Attributes:
 *
 * - Delay:  An ns3::Time specifying the speed of light transmission delay for
 *   the channel.
 *
 * \section PointToPointTracingModel Point-to-Point Tracing Model
 *
 * Like all ns-3 devices, the Point-to-Point Model provides a number of trace 
 * sources.  These trace sources can be hooked using your own custom trace code,
 * or you can use our helper functions to arrange for tracing to be enabled on 
 * devices you specify.
 *
 * \subsection PointToPointTracingModelUpperHooks Upper-Level (MAC) Hooks
 *
 * From the point of view of tracing in the net device, there are several 
 * interesting points to insert trace hooks.  A convention inherited from other
 * simulators is that packets destined for transmission onto attached networks
 * pass through a single "transmit queue" in the net device.  We provide trace 
 * hooks at this point in packet flow, which corresponds (abstractly) only to a 
 * transition from the network to data link layer, and call them collectively
 * the device MAC hooks.
 *
 * When a packet is sent to the Point-to-Point net device for transmission it 
 * always passes through the transmit queue.  The transmit queue in the 
 * PoiintToPointNetDevice inherits from Queue, and therefore inherits three 
 * trace sources:
 *
 * - An Enqueue operation source (see Queue::m_traceEnqueue);
 * - A Dequeue operation source (see Queue::m_traceDequeue);
 * - A Drop operation source (see Queue::m_traceDrop).
 *
 * The upper-level (MAC) trace hooks for the PointToPointNetDevice are, in fact, 
 * exactly these three trace sources on the single transmit queue of the device.  
 *
 * The m_traceEnqueue event is triggered when a packet is placed on the transmit
 * queue.  This happens at the time that ns3::PointtoPointNetDevice::Send or 
 * ns3::PointToPointNetDevice::SendFrom is called by a higher layer to queue a 
 * packet for transmission.  An Enqueue trace event firing should be interpreted
 * as only indicating that a higher level protocol has sent a packet to the device.
 *
 * The m_traceDequeue event is triggered when a packet is removed from the
 * transmit queue.  Dequeues from the transmit queue can happen in two
 * situations:  1) If the underlying channel is idle when 
 * PointToPointNetDevice::Send is called, a packet is dequeued from the transmit
 * queue and immediately transmitted;  2) a packet may be dequeued and 
 * immediately transmitted in an internal TransmitCompleteEvent that functions 
 * much  like a transmit complete interrupt service routine.  An Dequeue trace 
 * event firing may be viewed as indicating that the PointToPointNetDevice has
 * begun transmitting a packet.
 *
 * \subsection CsmaTracingModelUpperHooks Lower-Level (PHY) Hooks
 *
 * Similar to the upper level trace hooks, there are trace hooks available at
 * the lower levels of the net device.  We call these the PHY hooks.  These 
 * events fire from the device methods that talk directly to the 
 * PointToPointChannel.
 *
 * The trace source m_dropTrace is called to indicate a packet that is dropped
 * by the device.  This happens when a packet is discarded as corrupt due to a 
 * receive error model indication (see ns3::ErrorModel and the associated 
 * attribute "ReceiveErrorModel").
 *
 * The other low-level trace source fires on reception of a packet (see 
 * ns3::PointToPointNetDevice::m_rxTrace) from the PointToPointChannel.
 *
 * \section PointToPointModelSummary Point-To-Point Model Summary
 *
 * The ns3 Point-to-Point model is a simplistic model of a point to point
 * serial line link.
 *
 * Ns-3 Attributes provide a mechanism for setting various parameters in the 
 * device and channel such as data rates, speed-of-light delays and error model
 * selection.  Trace hooks are provided in the usual manner with a set of 
 * upper level hooks corresponding to a transmit queue and used in ASCII 
 * tracing; and also a set of lower level hooks used in pcap tracing.
 */
